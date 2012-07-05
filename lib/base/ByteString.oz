%%%
%%% Authors:
%%%   Kenny Chan <kennytm@gmail.com>
%%%
%%% Copyright:
%%%   Kenny Chan, 2012
%%%
%%% Last change:
%%%   $Date$ by $Author$
%%%   $Revision$
%%%
%%% This file is part of Mozart, an implementation
%%% of Oz 3
%%%    http://www.mozart-oz.org
%%%
%%% See the file "LICENSE" or
%%%    http://www.mozart-oz.org/LICENSE.html
%%% for information on usage and redistribution
%%% of this file, and for a DISCLAIMER OF ALL
%%% WARRANTIES.
%%%

%%
%% Module
%%

local
   fun {IsVariant Opt}
      Opt == littleEndian orelse Opt == bigEndian orelse Opt == bom
   end

   fun {Coder OptList Fun Input}
      Variants
      Encoding
   in
      case {List.partition OptList IsVariant Variants}
      of H|_ then
         Encoding = H
      [] nil then
         Encoding = utf8
      end

      {Fun Input Encoding Variants}
   end
in

   ByteString = byteString(
      is: IsByteString
      make: fun {$ V} {Boot_ByteString.encode V latin1 nil} end
      get: Boot_ByteString.get
      append: Boot_ByteString.append
      slice: Boot_ByteString.slice
      width: Boot_ByteString.length
      length: Boot_ByteString.length
      toString: fun {$ BS} {Boot_ByteString.decode BS latin1 nil} end
      %toStringWithTail: ---
      strchr: Boot_ByteString.strchr

      encode: fun {$ OptList V}
                 {Coder OptList Boot_ByteString.encode V}
              end
      decode: fun {$ OptList BS}
                 {Coder OptList Boot_ByteString.decode BS}
              end
   )

end